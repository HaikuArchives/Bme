#ifndef TAG_H
#define TAG_H

//TODO: add tag implementation as class/might add in separate file
/**	Tag structure to represent emoticons and other constructs within the text
 */
class Tag
{
	public:
		Tag();
		virtual			~Tag();
		
		int32			Type();
		BRect			Bounds();						
	
		virtual bool	IsSplittable();//=0?
		virtual void	Split();//=0?
		virtual void	DrawTag(BView *owner, BRect enclosingRect);	//=0?
	
	private:
		int32			m_start,
						m_end,
						m_type;
};

#endif